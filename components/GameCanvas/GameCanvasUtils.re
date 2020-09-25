// TODO: probably changable
let gridSize = 40;

[@bs.send]
external drawImage:
  (
    Webapi.Canvas.Canvas2d.t,
    ~image: Webapi.Dom.HtmlImageElement.t,
    ~dx: float,
    ~dy: float,
    ~dWidth: float=?,
    ~dHeight: float=?,
    ~sx: float=?,
    ~sy: float=?,
    ~sWidth: float=?,
    ~sHeight: float=?,
    unit
  ) =>
  unit;

type dimensions = {
  width: float,
  height: float,
};

let getElementByCords =
    (event: ReactEvent.Mouse.t, canvas: Webapi.Dom.Element.t) => {
  let (xBound, yBound) = (
    Webapi.Dom.Element.getBoundingClientRect(canvas) |> Webapi.Dom.DomRect.x,
    Webapi.Dom.Element.getBoundingClientRect(canvas) |> Webapi.Dom.DomRect.y,
  );
  let (x, y) = (
    ReactEvent.Mouse.clientX(event) - int_of_float(xBound),
    ReactEvent.Mouse.clientY(event) - int_of_float(yBound),
  );
  let (rowI, colI) = (y / gridSize, x / gridSize);

  (rowI, colI);
};

let drawOnCanvas =
    (
      ~context: Webapi.Canvas.Canvas2d.t,
      ~dimensions: dimensions,
      ~gameState,
      ~activeField: option(GameTypes.activeField),
    )
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
          | GameTypes.Path(path) =>
            let active = path.active;
            active
              ? setFillStyle(context, String, "rgba(255,0,0,0.2)")
              : setFillStyle(context, String, "rgba(255,255,255,0.05)");
            fillRect(
              context,
              ~x=colPos,
              ~y=rowPos,
              ~w=float_of_int(gridSize),
              ~h=float_of_int(gridSize),
            );
          | GameTypes.Node(node) =>
            let active = node.active;
            active
              ? setFillStyle(context, String, "rgba(255,0,0,0.7)")
              : setFillStyle(context, String, "rgba(255,255,255,0.1)");
            fillRect(
              context,
              ~x=colPos,
              ~y=rowPos,
              ~w=float_of_int(gridSize),
              ~h=float_of_int(gridSize),
            );
          };
          // print symbols
          switch (v) {
          | GameTypes.Path(path) =>
            switch (path.type_) {
            | Some(type_) =>
              let symbol = GameTypes.pathTypesToData(type_).visual;
              Webapi.Canvas.Canvas2d.setFillStyle(
                context,
                String,
                "rgb(255,255,255)",
              );
              Webapi.Canvas.Canvas2d.font(context, "30px Arial");
              Webapi.Canvas.Canvas2d.textAlign(context, "center");
              Webapi.Canvas.Canvas2d.textBaseline(context, "middle");
              switch (symbol) {
              | String(symbol) =>
                Webapi.Canvas.Canvas2d.fillText(
                  ~x=colPos +. 20.,
                  ~y=rowPos +. 20.,
                  symbol,
                  context,
                )
              | Svg(symbol) =>
                // TODO: remove js, probably extend webapi lib
                %raw
                {|
  (function a ( ) {
    var img = new Image();
img.onload = function() {
context.drawImage(img, colPos + 10, rowPos + 11);
}
img.src = symbol[0];
  })()
|}
              };
            | None => ()
            }
          | _ => ()
          };
        },
        v,
      ),
    gameState,
  );

  // highlight selected field
  switch (activeField) {
  | Some(field) =>
    let (rowI, colI) = field.cords;
    setFillStyle(context, String, "rgba(255,255,255,0.3)");
    fillRect(
      context,
      ~x=float_of_int(colI * gridSize),
      ~y=float_of_int(rowI * gridSize),
      ~w=float_of_int(gridSize),
      ~h=float_of_int(gridSize),
    );
  | None => ()
  };
};

// Extract canvas dimensions from canvas element
let canvasDimensions = (canvasElement: Dom.element): dimensions =>
  Webapi.Canvas.CanvasElement.{
    width: float_of_int(width(canvasElement)),
    height: float_of_int(height(canvasElement)),
  };

let draw = (~canvas, ~gameState, ~activeField) => {
  Webapi.Canvas.CanvasElement.(
    drawOnCanvas(
      ~context=getContext2d(canvas),
      ~dimensions=canvasDimensions(canvas),
      ~gameState,
      ~activeField,
    )
  );
};

let handleClick =
    (
      event,
      gameState,
      setGameState,
      canvas,
      setActiveField:
        (option(GameTypes.activeField) => option(GameTypes.activeField)) =>
        unit,
    ) => {
  let (rowI, colI) = getElementByCords(event, canvas);

  // TODO: Check if there is a safer way
  let element: GameTypes.node = gameState[rowI][colI];
  let newElement: GameTypes.node =
    switch (element) {
    | Node(node) => Node({active: !node.active})
    | Path(path) => Path({active: !path.active, type_: path.type_})
    };

  let newActiveField: option(GameTypes.activeField) =
    Some({node: newElement, cords: (rowI, colI)});

  setActiveField(_ => newActiveField);
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
          | _ => newGameState[x][y] = Path({active: node.active, type_: None})
          };
        };
      };
    }

  | _ => ()
  };

  setGameState(_ => newGameState);

  Webapi.Canvas.CanvasElement.(
    drawOnCanvas(
      ~context=getContext2d(canvas),
      ~dimensions=canvasDimensions(canvas),
      ~gameState,
      ~activeField=newActiveField,
    )
  );
};

let handleContextMenu =
    (
      ~event,
      ~canvas,
      ~gameState,
      ~activeField: option(GameTypes.activeField),
      ~setActiveField:
         (option(GameTypes.activeField) => option(GameTypes.activeField)) =>
         unit,
    ) => {
  let (rowI, colI) = getElementByCords(event, canvas);
  let element: GameTypes.node = gameState[rowI][colI];

  let newActiveField: option(GameTypes.activeField) =
    switch (activeField) {
    | Some(activeField) =>
      let (rowIActive, colIActive) = activeField.cords;
      switch (rowIActive === rowI && colIActive === colI) {
      | true => None
      | _ => Some({node: element, cords: (rowI, colI)})
      };
    | None => Some({node: element, cords: (rowI, colI)})
    };

  setActiveField(_ => newActiveField);

  Webapi.Canvas.CanvasElement.(
    drawOnCanvas(
      ~context=getContext2d(canvas),
      ~dimensions=canvasDimensions(canvas),
      ~gameState,
      ~activeField=newActiveField,
    )
  );
};
