let handleCheck =
    (
      value: bool,
      activeElement: GameTypes.activeField,
      setActiveField:
        (option(GameTypes.activeField) => option(GameTypes.activeField)) =>
        unit,
      gameState,
      setGameState,
    ) => {
  Js.log(value);
  let (rowI, colI) = activeElement.cords;

  setActiveField(_ =>
    switch (activeElement.node) {
    | Path(_) =>
      Some({node: Path({active: value}), cords: activeElement.cords})
    | Node(_) =>
      Some({node: Node({active: value}), cords: activeElement.cords})
    }
  );

  // change game state

  let newGameState = gameState;
  switch (activeElement.node) {
  | Path(_) => newGameState[rowI][colI] = GameTypes.Path({active: value})
  | Node(_) => newGameState[rowI][colI] = GameTypes.Node({active: value})
  };

  setGameState(_ => newGameState);
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
    ) => {
  let handleActiveChange = (ev, field) =>
    handleCheck(
      ReactEvent.Form.target(ev)##checked,
      field,
      setActiveField,
      gameState,
      setGameState,
    );
  <div
    className=[%tw
      "flex items-center  flex-wrap bg-white p-6 mt-12 mx-12 rounded-lg"
    ]>
    <div className=[%tw "flex h-full"]>
      <Button
        text="Restart"
        onClick={_ => {
          setGameState(_ => {PuzzleBuilderUtils.createInitialState()});
          setActiveField(_ => None);
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
          switch (view) {
          | Builder => setView(_ => GameTypes.Player)
          | Player => setView(_ => GameTypes.Builder)
          }
        }}
        className=[%tw "mr-1"]
      />
      <div
        className=[%tw "bg-gray-900 h-full rounded mx-2"]
        style={ReactDOMRe.Style.make(~width="2px", ())}
      />
    </div>
    <div className=[%tw "flex items-center"]>
      {switch (activeField) {
       | Some(field) =>
         switch (field.node) {
         | Path(path) =>
           <>
             <div className=[%tw "font-extrabold mr-2"]>
               {React.string("Path")}
             </div>
             <label className="inline-flex items-center">
               <input
                 type_="checkbox"
                 checked={path.active}
                 onChange={ev => handleActiveChange(ev, field)}
               />
               <span className="ml-2 text-gray-700">
                 {React.string("Active")}
               </span>
             </label>
           </>
         | Node(node) =>
           <>
             <div className=[%tw "font-extrabold mr-2"]>
               {React.string("Node")}
             </div>
             <label className="inline-flex items-center">
               <input
                 type_="checkbox"
                 checked={node.active}
                 onChange={ev => handleActiveChange(ev, field)}
               />
               <span className="ml-2 text-gray-700">
                 {React.string("Active")}
               </span>
             </label>
           </>
         }
       | _ => React.null
       }}
    </div>
  </div>;
};
