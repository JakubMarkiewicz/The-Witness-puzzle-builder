[@react.component]
let make = () => {
  // TODO: fix forcing update
  let (forceUpdate, setForceUpdate) = React.useState(_ => 0);
  let (gameState, setGameState) =
    React.useState(_ => PuzzleBuilderUtils.createInitialState());
  let (activeField, setActiveField) = React.useState(_ => None);
  let (view, setView) = React.useState(_ => GameTypes.Builder);
  <div
    className=[%tw "h-full flex flex-col bg-gray-300 mx-4"]
    style={ReactDOMRe.Style.make(
      ~borderTopLeftRadius="90px",
      ~borderTopRightRadius="90px",
      (),
    )}>
    <MenuBar
      gameState
      setGameState
      setForceUpdate
      view
      setView
      activeField
      setActiveField
    />
    <div className=[%tw "h-full w-full flex justify-center items-center"]>
      <div
        className=[%tw "shadow-xl p-4 rounded-lg relative"]
        style={ReactDOMRe.Style.make(~background="#ffa36c", ())}>
        <GameCanvas
          forceUpdate
          gameState
          setGameState
          view
          activeField
          setActiveField
        />
      </div>
    </div>
  </div>;
};
