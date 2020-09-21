[@react.component]
let make = () => {
  <div className=[%tw "h-full flex flex-col"]>
    <Header />
    <div className=[%tw "h-full"]> <PuzzleBuilder /> </div>
  </div>;
};

let default = make;
