let handleTypeChangeNode =
    (
      value: option(GameTypes.nodeTypes),
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
    | Path(path) =>
      Some({
        node: Path({active: path.active, type_: path.type_}),
        cords: activeElement.cords,
      })
    | Node(node) =>
      Some({
        node: Node({active: node.active, type_: value}),
        cords: activeElement.cords,
      })
    }
  );

  let newGameState = gameState;
  switch (activeElement.node) {
  | Path(path) =>
    newGameState[rowI][colI] =
      GameTypes.Path({active: path.active, type_: path.type_})
  | Node(node) =>
    newGameState[rowI][colI] =
      GameTypes.Node({active: node.active, type_: value})
  };

  setGameState(_ => newGameState);
  setForceUpdate(v => v + 1);
};

[@react.component]
let make =
    (
      ~node: GameTypes.testNode,
      ~field: GameTypes.activeField,
      ~handleActiveChange,
      ~activeField: option(GameTypes.activeField),
      ~setActiveField,
      ~gameState,
      ~setGameState,
      ~setForceUpdate,
    ) =>
  <>
    <div className=[%tw "font-extrabold mr-2"]> {React.string("Node")} </div>
    <Tippy
      className=[%tw "bg-white"]
      content={
        <div>
          {[|GameTypes.Color(None)|]
           |> Array.map((nodeType: GameTypes.nodeTypes) => {
                let data = GameTypes.nodeTypesToData(nodeType);
                let active =
                  switch (activeField) {
                  | Some(activeField) =>
                    switch (activeField.node) {
                    | Node(node) when node.type_ === Some(nodeType) => true
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
                    handleTypeChangeNode(
                      active ? None : Some(Color(Some(White))),
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
      <div className=[%tw "px-4 py-1 rounded border-2 bg-white"]>
        {Belt.Option.mapWithDefault(
           node.type_, "Select type", (type_: GameTypes.nodeTypes) =>
           GameTypes.nodeTypesToData(type_).string
         )
         |> React.string}
      </div>
    </Tippy>
    {switch (activeField) {
     | Some(field) =>
       switch (field.node) {
       | Node(node) =>
         switch (node.type_) {
         | Some(Color(color)) =>
           <MenuBarColor
             color
             activeField
             setActiveField
             gameState
             setGameState
             setForceUpdate
           />
         | _ => React.null
         }
       | _ => React.null
       }
     | _ => React.null
     }}
  </>;
