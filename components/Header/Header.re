[@react.component]
let make = () =>
  <nav
    className=[%tw
      "flex items-center justify-between flex-wrap bg-teal-500 p-6"
    ]>
    <Next.Link href="/editor"> <a> {React.string("Editor")} </a> </Next.Link>
  </nav>;
