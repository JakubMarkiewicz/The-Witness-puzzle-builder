let handleCheck =
    (
      value: bool,
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
        node: Path({active: value, type_: path.type_}),
        cords: activeElement.cords,
      })
    | Node(node) =>
      Some({
        node: Node({active: value, type_: node.type_}),
        cords: activeElement.cords,
      })
    }
  );

  // change game state

  let newGameState = gameState;
  switch (activeElement.node) {
  | Path(path) =>
    newGameState[rowI][colI] =
      GameTypes.Path({active: value, type_: path.type_})
  | Node(node) =>
    newGameState[rowI][colI] =
      GameTypes.Node({active: value, type_: node.type_})
  };

  setGameState(_ => newGameState);
  setForceUpdate(v => v + 1);
};

module Internal = {
  module VisualElement = {
    [@react.component]
    let make = (~visual: GameTypes.visualType) =>
      switch (visual) {
      | String(v) => React.string(v)
      | Svg(svgPath) => <img src=svgPath />
      };
  };
};

[@react.component]
let make =
    (
      ~gameState,
      ~setGameState,
      ~view,
      ~setView,
      ~activeField: option(GameTypes.activeField),
      ~setActiveField,
      ~setForceUpdate,
    ) => {
  let handleActiveChange = (ev, field) =>
    handleCheck(
      ReactEvent.Form.target(ev)##checked,
      field,
      setActiveField,
      gameState,
      setGameState,
      setForceUpdate,
    );
  <div className=[%tw "relative"]>
    <div
      className=[%tw
        "flex items-center  flex-wrap bg-white p-6 mt-12 mx-12 rounded-lg shadow-lg"
      ]>
      <div className=[%tw "flex h-full"]>
        <Button
          text="Restart"
          onClick={_ => {
            setGameState(_ => PuzzleBuilderUtils.createInitialState());
            setActiveField(_ => None);
            setForceUpdate(v => v + 1);
          }}
          className=[%tw "mr-1"]
          disabled={view === GameTypes.Player}
        />
        <Button
          text={
            switch (view) {
            | Builder => "Play"
            | Player => "Build"
            }
          }
          onClick={_ => {
            setActiveField(_ => None);
            setForceUpdate(v => v + 1);
            switch (view) {
            | Builder => setView(_ => GameTypes.Player)
            | Player => setView(_ => GameTypes.Builder)
            };
          }}
          className=[%tw "mr-1"]
        />
        <div
          className=[%tw "bg-gray-900 h-full rounded mx-2"]
          style={ReactDOMRe.Style.make(~width="2px", ())}
        />
      </div>
      <div className=[%tw "flex items-center flex-1"]>
        {switch (activeField) {
         | Some(field) =>
           switch (field.node) {
           | Path(path) =>
             <MenuBarPath
               path
               field
               handleActiveChange
               activeField
               setActiveField
               gameState
               setGameState
               setForceUpdate
             />
           | Node(node) =>
             <MenuBarNode
               node
               field
               handleActiveChange
               activeField
               setActiveField
               gameState
               setGameState
               setForceUpdate
             />
           }
         | _ => React.null
         }}
      </div>
      <div>
        <Button text="Copy playable link" onClick={_ => ()} disabled=true />
      </div>
    </div>
    <MenuBarInfo />
  </div>;
};
