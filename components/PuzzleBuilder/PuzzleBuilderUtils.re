let createInitialState = () =>
  Belt.Array.make(15, Belt.Array.make(15, None))
  |> Array.mapi((rowI, rowV) =>
       switch (rowI mod 2 === 0) {
       | true => Array.map(_ => GameTypes.Path({active: false}), rowV)
       | _ =>
         Array.mapi(
           (colI, _) =>
             switch (colI mod 2 !== 0) {
             | true => GameTypes.Node({active: false})
             | _ => GameTypes.Path({active: false})
             },
           rowV,
         )
       }
     );
