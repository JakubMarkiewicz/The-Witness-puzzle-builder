external convertTargetToDomElement: Js.t({..}) => Webapi.Dom.Element.t =
  "%identity";

let handleMouseDown = (~ev, ~gameState, ~setPainting) => {
  let canvas = ReactEvent.Mouse.target(ev) |> convertTargetToDomElement;
  let dimensions = GameCanvasUtils.canvasDimensions(canvas);
  let context = Webapi.Canvas.CanvasElement.getContext2d(canvas);

  let (rowI, colI) = GameCanvasUtils.getElementByCords(ev, canvas);
  switch (gameState[rowI][colI]) {
  | GameTypes.Path(path) =>
    switch (path.type_) {
    | Some(GameTypes.Start) =>
      // clear canvas
      Webapi.Canvas.Canvas2d.clearRect(
        context,
        ~x=0.,
        ~y=0.,
        ~w=dimensions.width,
        ~h=dimensions.height,
      );
      setPainting(_ => true);
    | _ => ()
    }
  | _ => ()
  };
};

let handleOnMouseMove = (~ev, ~gameState, ~painting, ~path, ~setPath) => {
  switch (painting) {
  | true =>
    let canvas = ReactEvent.Mouse.target(ev) |> convertTargetToDomElement;
    let context = Webapi.Canvas.CanvasElement.getContext2d(canvas);
    let (rowI, colI) = GameCanvasUtils.getElementByCords(ev, canvas);
    // add to path
    let inPath = path |> Js.Array.some(((x, y)) => x === rowI && y === colI);
    let inGamePath =
      switch (gameState[rowI][colI]) {
      | GameTypes.Path(p) => p.active
      | _ => false
      };
    let nextElementConnected =
      switch (Array.length(path)) {
      | l when l > 0 =>
        let (prevElementRowI, prevElementColI) = path[Array.length(path) - 1];
        let horizontal =
          abs(prevElementRowI - rowI) <= 1 && prevElementColI === colI;
        let vertical =
          abs(prevElementColI - colI) <= 1 && prevElementRowI === rowI;
        horizontal || vertical;
      | _ => true
      };

    switch (nextElementConnected, inPath, inGamePath) {
    | (true, false, true) =>
      setPath(path => Array.concat([path, [|(rowI, colI)|]]));
      // paint path
      Webapi.Canvas.Canvas2d.setFillStyle(context, String, "#ebdc87");
      Webapi.Canvas.Canvas2d.fillRect(
        context,
        ~x=float_of_int(colI * 40),
        ~y=float_of_int(rowI * 40),
        ~w=float_of_int(40),
        ~h=float_of_int(40),
      );
    | (_, true, _) =>
      // handle going back
      let (lastX, lastY) = path[Array.length(path) - 1];
      let isLastElement = rowI === lastX && colI === lastY;
      switch (isLastElement) {
      | false =>
        let indexOfMouseElement =
          Js.Array.findIndex(((x, y)) => rowI === x && colI === y, path);

        let pathToRemove =
          Js.Array.slice(
            ~start=indexOfMouseElement + 1,
            ~end_=Array.length(path),
            path,
          );
        Js.Array.forEach(
          ((rowI, colI)) => {
            Webapi.Canvas.Canvas2d.clearRect(
              context,
              ~x=float_of_int(colI * 40),
              ~y=float_of_int(rowI * 40),
              ~w=40.,
              ~h=40.,
            )
          },
          pathToRemove,
        );
        setPath(path =>
          Js.Array.slice(~start=0, ~end_=indexOfMouseElement + 1, path)
        );
      | _ => ()
      };
    | _ => ()
    };
  | _ => ()
  };
};

let paintValidatedPath = (~canvas, ~path, ~valid) => {
  let context = Webapi.Canvas.CanvasElement.getContext2d(canvas);
  let dimensions = GameCanvasUtils.canvasDimensions(canvas);
  Webapi.Canvas.Canvas2d.clearRect(
    context,
    ~x=0.,
    ~y=0.,
    ~w=dimensions.width,
    ~h=dimensions.height,
  );
  switch (valid) {
  | true => Webapi.Canvas.Canvas2d.setFillStyle(context, String, "#799351")
  | _ => Webapi.Canvas.Canvas2d.setFillStyle(context, String, "#d54062")
  };

  Js.Array.forEach(
    ((rowI, colI)) =>
      Webapi.Canvas.Canvas2d.fillRect(
        context,
        ~x=float_of_int(colI * 40),
        ~y=float_of_int(rowI * 40),
        ~w=float_of_int(40),
        ~h=float_of_int(40),
      ),
    path,
  );
};

let validateHexagons = (~gameState, ~path) => {
  let hexagons =
    gameState
    |> Js.Array.reducei(
         (acc: array((int, int)), v, rowI) => {
           let vals =
             Js.Array.reducei(
               (accInner, v, colI) =>
                 switch (v) {
                 | GameTypes.Path(path) =>
                   switch (path.type_) {
                   | Some(GameTypes.Hexagon) =>
                     Array.concat([accInner, [|(rowI, colI)|]])
                   | _ => accInner
                   }
                 | _ => accInner
                 },
               [||],
               v,
             );
           Array.concat([acc, vals]);
         },
         [||],
       );

  let allHexagonsValid: bool =
    hexagons
    |> Js.Array.every(((rowI, colI)) =>
         Js.Array.some(
           ((pathRowI, pathColI)) => rowI === pathRowI && colI === pathColI,
           path,
         )
       );

  allHexagonsValid;
};

let validatePath = (~canvas, ~gameState, ~path) => {
  let (rowI, colI) = path[Array.length(path) - 1];
  let validEnd =
    switch (gameState[rowI][colI]) {
    | GameTypes.Path(p) =>
      switch (p.type_) {
      | Some(End) => validateHexagons(~gameState, ~path)
      | _ => false
      }
    | _ => false
    };

  switch (validEnd) {
  | true => paintValidatedPath(~canvas, ~path, ~valid=true)
  | _ => paintValidatedPath(~canvas, ~path, ~valid=false)
  };
};

let handleOnMouseUp = (~ev, ~gameState, ~setPainting, ~setPath, ~path) => {
  let canvas = ReactEvent.Mouse.target(ev) |> convertTargetToDomElement;
  switch (path) {
  | p when Array.length(p) > 0 => validatePath(~canvas, ~gameState, ~path)
  | _ => ()
  };

  setPainting(_ => false);
  setPath(_ => [||]);
};
