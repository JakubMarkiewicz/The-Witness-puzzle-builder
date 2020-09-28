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

let validateColors = (~gameState, ~seperatedAreas) =>
  seperatedAreas
  |> Array.map(area => {
       let areaNodes =
         area
         |> Array.map(((row, col)) => gameState[row][col])
         |> Js.Array.map((v: GameTypes.node) =>
              switch (v) {
              | Node(n) =>
                switch (n.type_) {
                | Some(Color(color)) => color
                | _ => None
                }
              | _ => None
              }
            )
         |> Js.Array.filter(v => v !== None);

       let res =
         areaNodes
         |> Js.Array.reducei(
              (acc, v, i) =>
                switch (i) {
                | 0 => true
                | _ when acc =>
                  switch (v, areaNodes[i - 1]) {
                  | (Some(color), Some(prevColor)) => color === prevColor
                  | _ => false
                  }
                | _ => false
                },
              false,
            );

       res;
     })
  |> Js.Array.every(v => v === true);
