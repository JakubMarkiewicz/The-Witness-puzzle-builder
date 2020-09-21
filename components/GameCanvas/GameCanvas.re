open GameTypes;

module Styles = {
  let canvasStyle = [%tw "bg-gray-900"];
};

type dimensions = {
  width: float,
  height: float,
};

// TODO: probably changable
let gridSize = 40;

let elementByCords = (event, canvas) => {
  let (xBound, yBound) = (
    Webapi.Dom.Element.getBoundingClientRect(canvas) |> Webapi.Dom.DomRect.x,
    Webapi.Dom.Element.getBoundingClientRect(canvas) |> Webapi.Dom.DomRect.y,
  );
  let (x, y) = (
    ReactEvent.Mouse.clientX(event) - int_of_float(xBound),
    ReactEvent.Mouse.clientY(event) - int_of_float(yBound),
  );
  let (rowI, colI) = (x / gridSize, y / gridSize);

  (rowI, colI);
};

let drawOnCanvas =
    (context: Webapi.Canvas.Canvas2d.t, dimensions: dimensions, gameState)
    : unit => {
  open Webapi.Canvas.Canvas2d;
  clearRect(context, ~x=0., ~y=0., ~w=dimensions.width, ~h=dimensions.height);

  setFillStyle(context, String, "rgba(255,255,255,0.03)");
  for (i in 1 to int_of_float(dimensions.width) / gridSize) {
    fillRect(
      context,
      ~x=float_of_int(i) *. float_of_int(gridSize),
      ~y=0.0,
      ~w=1.0,
      ~h=dimensions.width,
    );
    fillRect(
      context,
      ~x=0.,
      ~y=float_of_int(i) *. float_of_int(gridSize),
      ~w=dimensions.width,
      ~h=1.0,
    );
  };
  Js.Array.forEachi(
    (v, rowI) =>
      Js.Array.forEachi(
        (v, colI) => {
          let (rowPos, colPos) = (
            float_of_int(rowI * gridSize),
            float_of_int(colI * gridSize),
          );
          switch (v) {
          | Path(path) =>
            let active = path.active;
            active
              ? setFillStyle(context, String, "rgba(255,0,0,0.2)")
              : setFillStyle(context, String, "rgba(255,255,255,0.05)");
            fillRect(
              context,
              ~x=rowPos,
              ~y=colPos,
              ~w=float_of_int(gridSize),
              ~h=float_of_int(gridSize),
            );
          | Node(node) =>
            let active = node.active;
            active
              ? setFillStyle(context, String, "rgba(255,0,0,0.7)")
              : setFillStyle(context, String, "rgba(255,255,255,0.1)");
            fillRect(
              context,
              ~x=rowPos,
              ~y=colPos,
              ~w=float_of_int(gridSize),
              ~h=float_of_int(gridSize),
            );
          };
        },
        v,
      ),
    gameState,
  );
};

// Extract canvas dimensions from canvas element
let canvasDimensions = (canvasElement: Dom.element): dimensions =>
  Webapi.Canvas.CanvasElement.{
    width: float_of_int(width(canvasElement)),
    height: float_of_int(height(canvasElement)),
  };

let draw = (canvas, gameState) => {
  Webapi.Canvas.CanvasElement.(
    drawOnCanvas(getContext2d(canvas), canvasDimensions(canvas), gameState)
  );
};

let handleClick = (event, gameState, setGameState, canvas) => {
  let (rowI, colI) = elementByCords(event, canvas);

  // TODO: Check if there is a safer way
  let element: node = gameState[rowI][colI];
  let newElement: node =
    switch (element) {
    | Node(node) => Node(node.active ? {active: false} : {active: true})
    | Path(path) => Path(path.active ? {active: false} : {active: true})
    };

  let newGameState = gameState;
  // TODO: Check if there is a safer way
  newGameState[rowI][colI] = newElement;

  // handle paths
  switch (newElement) {
  | Node(node) =>
    for (row in (-1) to 1) {
      for (col in (-1) to 1) {
        switch (row, col) {
        | (row, col) when row === 0 && col === 0 => ()
        | _ =>
          let (x, y) = (rowI + row, colI + col);
          let activeAround =
            Belt.Array.makeBy(9, v => (x - 1 + v mod 3, y - 1 + v / 3))
            |> Js.Array.filter(((xNew, yNew)) => {
                 let inBounds =
                   xNew >= 0 && yNew >= 0 && xNew <= 15 && yNew <= 15;
                 let selectedNode = xNew === rowI && yNew === colI;
                 inBounds && !selectedNode;
               })
            |> Js.Array.filter(((xNew, yNew)) =>
                 switch (newGameState[xNew][yNew]) {
                 | Node(node) => node.active
                 | _ => false
                 }
               )
            |> Js.Array.length > 0;
          switch (activeAround) {
          | true => ()
          | _ => newGameState[x][y] = Path({active: node.active})
          };
        };
      };
    }

  | _ => ()
  };

  setGameState(_ => newGameState);

  Webapi.Canvas.CanvasElement.(
    drawOnCanvas(getContext2d(canvas), canvasDimensions(canvas), gameState)
  );
};

let handleContextMenu =
    (~event, ~canvas, ~gameState, ~activeField, ~setActiveField) => {
  let (rowI, colI) = elementByCords(event, canvas);
  let element: node = gameState[rowI][colI];
  switch (activeField) {
  | Some(activeField) => setActiveField(_ => None)
  | None => setActiveField(_ => Some({node: element, cords: (rowI, colI)}))
  };

  Webapi.Canvas.CanvasElement.(
    drawOnCanvas(getContext2d(canvas), canvasDimensions(canvas), gameState)
  );
};

external convertTargetToDomElement: Js.t({..}) => Webapi.Dom.Element.t =
  "%identity";

[@react.component]
let make = (~gameState, ~setGameState, ~view, ~activeField, ~setActiveField) => {
  let canvasRef = React.useRef(Js.Nullable.null);

  React.useEffect1(
    _ => {
      switch (React.Ref.current(canvasRef) |> Js.Nullable.toOption) {
      | Some(canvasRef) => draw(canvasRef, gameState)
      | _ => ()
      };
      None;
    },
    [|gameState|],
  );
  <canvas
    ref={ReactDOMRe.Ref.domRef(canvasRef)}
    width="600px"
    height="600px"
    className=Styles.canvasStyle
    onClick={ev =>
      switch (view) {
      | Builder =>
        handleClick(
          ev,
          gameState,
          setGameState,
          ReactEvent.Mouse.target(ev) |> convertTargetToDomElement,
        )
      | Player => ()
      }
    }
    onContextMenu={ev => {
      ReactEvent.Mouse.preventDefault(ev);
      handleContextMenu(
        ~event=ev,
        ~canvas=ReactEvent.Mouse.target(ev) |> convertTargetToDomElement,
        ~gameState,
        ~activeField,
        ~setActiveField,
      );
    }}
  />;
};

let default = make;
