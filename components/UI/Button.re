[@react.component]
let make = (~text, ~onClick, ~className="", ~disabled=false) =>
  <button
    className=Cn.(
      [%tw
        "bg-blue-500 hover:bg-blue-700 text-white font-bold py-2 px-4 rounded"
      ]
      + [%tw "cursor-not-allowed opacity-50"]->on(disabled)
      + className
    )
    onClick
    disabled>
    {React.string(text)}
  </button>;
