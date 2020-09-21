open GameTypes;

let createInitialState = () =>
  Belt.Array.make(15, Belt.Array.make(15, None))
  |> Array.mapi((rowI, rowV) =>
       switch (rowI mod 2 === 0) {
       | true => Array.map(_ => Path({active: false}), rowV)
       | _ =>
         Array.mapi(
           (colI, _) =>
             switch (colI mod 2 !== 0) {
             | true => Node({active: false})
             | _ => Path({active: false})
             },
           rowV,
         )
       }
     );

[@react.component]
let make = () => {
  let (gameState, setGameState) = React.useState(_ => createInitialState());
  let (activeField, setActiveField) = React.useState(_ => None);
  let (view, setView) = React.useState(_ => Builder);
  <div className=[%tw "h-full flex flex-col bg-gray-900"]>
    <div
      className=[%tw
        "flex items-center justify-between flex-wrap bg-white p-6 mt-12 mx-12 rounded-lg"
      ]>
      <div className=[%tw "flex h-full"]>
        <Button
          text="Restart"
          onClick={_ => setGameState(_ => createInitialState())}
          className=[%tw "mr-1"]
          disabled={view === Player}
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
            | Builder => setView(_ => Player)
            | Player => setView(_ => Builder)
            }
          }}
          className=[%tw "mr-1"]
        />
        <div
          className=[%tw "bg-gray-900 h-full rounded mx-2"]
          style={ReactDOMRe.Style.make(~width="2px", ())}
        />
      </div>
    </div>
    <div className=[%tw "h-full w-full flex justify-center items-center"]>
      <GameCanvas gameState setGameState view activeField setActiveField />
    </div>
  </div>;
};
