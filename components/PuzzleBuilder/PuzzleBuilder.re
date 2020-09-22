[@react.component]
let make = () => {
  let (gameState, setGameState) =
    React.useState(_ => PuzzleBuilderUtils.createInitialState());
  let (activeField, setActiveField) = React.useState(_ => None);
  let (view, setView) = React.useState(_ => GameTypes.Builder);
  <div className=[%tw "h-full flex flex-col bg-gray-900"]>
    <MenuBar gameState setGameState view setView activeField setActiveField />
    <div className=[%tw "h-full w-full flex justify-center items-center"]>
      <GameCanvas gameState setGameState view activeField setActiveField />
    </div>
  </div>;
};
