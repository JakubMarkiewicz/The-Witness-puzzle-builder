[@react.component]
let make = (~text, ~onClick, ~className="", ~disabled=false) =>
  <button
    className=Cn.(
      [%tw
        "bg-orange-500 hover:bg-orange-700 text-white font-bold py-2 px-4 rounded transition duration-300"
      ]
      + [%tw "cursor-not-allowed opacity-50"]->on(disabled)
      + className
    )
    onClick
    disabled>
    {React.string(text)}
  </button>;
