[@react.component]
let make = () => {
  <>
    <Header />
    <div className=[%tw "flex w-full h-full justify-center items-center"]>
      <img src="/static/empty.svg" />
    </div>
  </>;
};

let default = make;
