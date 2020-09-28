let handleTypeChange =
    (
      value: option(GameTypes.pathTypes),
      activeElement: GameTypes.activeField,
      setActiveField:
        (option(GameTypes.activeField) => option(GameTypes.activeField)) =>
        unit,
      gameState,
      setGameState,
      setForceUpdate,
    ) => {
  let (rowI, colI) = activeElement.cords;

  setActiveField(_ =>
    switch (activeElement.node) {
    | Path(_) =>
      Some({
        node: Path({active: true, type_: value}),
        cords: activeElement.cords,
      })
    | Node(node) =>
      Some({
        node: Node({active: node.active, type_: node.type_}),
        cords: activeElement.cords,
      })
    }
  );

  let newGameState = gameState;
  switch (activeElement.node) {
  | Path(_) =>
    newGameState[rowI][colI] = GameTypes.Path({active: true, type_: value})
  | Node(node) =>
    newGameState[rowI][colI] =
      GameTypes.Node({active: node.active, type_: node.type_})
  };

  setGameState(_ => newGameState);
  setForceUpdate(v => v + 1);
};

[@react.component]
let make =
    (
      ~path: GameTypes.testPath,
      ~field: GameTypes.activeField,
      ~handleActiveChange,
      ~activeField: option(GameTypes.activeField),
      ~setActiveField,
      ~gameState,
      ~setGameState,
      ~setForceUpdate,
    ) =>
  <>
    <div className=[%tw "font-extrabold mr-2"]> {React.string("Path")} </div>
    <label className="inline-flex items-center">
      <input
        type_="checkbox"
        checked={path.active}
        onChange={ev => handleActiveChange(ev, field)}
      />
      <span className="mx-2 text-gray-700"> {React.string("Active")} </span>
    </label>
    <Tippy
      className=[%tw "bg-white"]
      content={
        <div>
          {[|GameTypes.Start, GameTypes.End, GameTypes.Hexagon|]
           |> Array.map((pathType: GameTypes.pathTypes) => {
                let data = GameTypes.pathTypesToData(pathType);
                let active =
                  switch (activeField) {
                  | Some(activeField) =>
                    switch (activeField.node) {
                    | Path(path) when path.type_ === Some(pathType) => true
                    | _ => false
                    }
                  | None => false
                  };
                <div
                  className=Cn.(
                    [%tw
                      "flex justify-between w-40 p-4 rounded cursor-pointer"
                    ]
                    + [%tw "hover:bg-gray-200"]->on(!active)
                    + [%tw "bg-gray-400 hover:bg-red-200"]->on(active)
                  )
                  onClick={_ =>
                    handleTypeChange(
                      active ? None : Some(pathType),
                      field,
                      setActiveField,
                      gameState,
                      setGameState,
                      setForceUpdate,
                    )
                  }>
                  <div> <MenuBarVisualElement visual={data.visual} /> </div>
                  <div> {React.string(data.string)} </div>
                </div>;
              })
           |> React.array}
        </div>
      }
      arrow=false
      theme="light"
      placement="bottom"
      interactive=true>
      <div className=[%tw "px-4 py-1 rounded border-2"]>
        {Belt.Option.mapWithDefault(
           path.type_, "Select type", (type_: GameTypes.pathTypes) =>
           GameTypes.pathTypesToData(type_).string
         )
         |> React.string}
      </div>
    </Tippy>
  </>;
