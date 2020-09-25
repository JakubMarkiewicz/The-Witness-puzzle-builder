type instruction = {
  key: string,
  explanation: string,
};

let instructions = [|
  {key: "LMB", explanation: "Activate/Deactivate node"},
  {key: "RMB", explanation: "Select/Deselect node"},
|];

[@react.component]
let make = () => {
  let (active, setActive) = React.useState(_ => false);
  <div className=[%tw "absolute left-0 right-0 flex justify-center"]>
    <div
      className=[%tw
        "bg-white p-1 w-8 h-8 rounded-full flex justify-center items-center -mt-4"
      ]>
      <img
        src="/static/info.svg"
        className=[%tw "cursor-pointer z-10"]
        onClick={_ => setActive(v => !v)}
      />
    </div>
    {switch (active) {
     | true =>
       <div
         className=[%tw
           "flex items-center  flex-wrap bg-white p-6 mx-12 left-0 right-0 rounded-lg rounded-t-sm -mt-4 shadow-lg absolute"
         ]>
         {instructions
          |> Array.map(v =>
               <div className=[%tw "mr-4"]>
                 <div className=[%tw "font-extrabold"]>
                   {React.string(v.key)}
                 </div>
                 <div> {React.string(v.explanation)} </div>
               </div>
             )
          |> React.array}
       </div>
     | _ => React.null
     }}
  </div>;
};
