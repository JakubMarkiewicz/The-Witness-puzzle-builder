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
    | Node(_) =>
      Some({node: Node({active: value}), cords: activeElement.cords})
    }
  );

  // change game state

  let newGameState = gameState;
  switch (activeElement.node) {
  | Path(path) =>
    newGameState[rowI][colI] =
      GameTypes.Path({active: value, type_: path.type_})
  | Node(_) => newGameState[rowI][colI] = GameTypes.Node({active: value})
  };

  setGameState(_ => newGameState);
  setForceUpdate(v => v + 1);
};

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
    | Path(path) =>
      Some({
        node: Path({active: path.active, type_: value}),
        cords: activeElement.cords,
      })
    | Node(node) =>
      Some({node: Node({active: node.active}), cords: activeElement.cords})
    }
  );

  let newGameState = gameState;
  switch (activeElement.node) {
  | Path(path) =>
    newGameState[rowI][colI] =
      GameTypes.Path({active: path.active, type_: value})
  | Node(node) =>
    newGameState[rowI][colI] = GameTypes.Node({active: node.active})
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
                 <span className="mx-2 text-gray-700">
                   {React.string("Active")}
                 </span>
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
                               | Path(path)
                                   when path.type_ === Some(pathType) =>
                                 true
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
                               + [%tw "bg-gray-400 hover:bg-red-200"]
                                 ->on(active)
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
                             <div>
                               <Internal.VisualElement visual={data.visual} />
                             </div>
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
      <div>
        <Button text="Copy playable link" onClick={_ => ()} disabled=true />
      </div>
    </div>
    <MenuBarInfo />
  </div>;
};
