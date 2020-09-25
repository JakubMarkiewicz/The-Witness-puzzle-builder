[@react.component]
let make = () =>
  <nav className=[%tw "p-6 flex justify-center"]>
    <div
      className=[%tw
        "flex items-center justify-between flex-wrap max-w-xl w-full font-extrabold"
      ]>
      <Next.Link href="/learn"> <a> {React.string("Learn")} </a> </Next.Link>
      <Next.Link href="/editor"> <a> {React.string("Edit")} </a> </Next.Link>
      <Next.Link href="/play"> <a> {React.string("Play")} </a> </Next.Link>
    </div>
  </nav>;
