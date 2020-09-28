let handleColorChange =
    (
      ~color: option(GameTypes.nodeColor),
      ~activeField: option(GameTypes.activeField),
      ~setActiveField:
         (option(GameTypes.activeField) => option(GameTypes.activeField)) =>
         unit,
      ~gameState,
      ~setGameState,
      ~setForceUpdate,
    ) => {
  setActiveField((v: option(GameTypes.activeField)) => {
    switch (v) {
    | Some(activeField) =>
      switch (activeField.node, color) {
      | (Node(node), None) =>
        Some({
          node: Node({active: node.active, type_: None}),
          cords: activeField.cords,
        })
      | (Node(node), Some(_)) =>
        Some({
          node: Node({active: node.active, type_: Some(Color(color))}),
          cords: activeField.cords,
        })
      | _ => v
      }
    | _ => v
    }
  });

  let newGameState = gameState;
  switch (activeField) {
  | Some(field) =>
    let (row, col) = field.cords;
    switch (activeField) {
    | Some(activeField) =>
      switch (activeField.node) {
      | Node(node) =>
        newGameState[row][col] =
          GameTypes.Node({active: node.active, type_: Some(Color(color))})
      | _ => ()
      }
    | _ => ()
    };
  | _ => ()
  };
  setGameState(_ => newGameState);

  setForceUpdate(v => v + 1);
};

[@react.component]
let make =
    (
      ~color,
      ~activeField,
      ~setActiveField,
      ~gameState,
      ~setGameState,
      ~setForceUpdate,
    ) => {
  let colors = [|GameTypes.White, GameTypes.Black, GameTypes.Green|];

  <Tippy
    className=[%tw "bg-white p-4"]
    arrow=false
    theme="light"
    placement="bottom"
    interactive=true
    content={
      <div className=[%tw "flex"]>
        {colors
         |> Array.map(v => {
              let active =
                switch ((activeField: option(GameTypes.activeField))) {
                | Some(activeField) =>
                  switch (activeField.node) {
                  | Node(n) =>
                    switch (n.type_) {
                    | Some(Color(_)) => true
                    | _ => false
                    }
                  | _ => false
                  }
                | _ => false
                };
              <div
                className=[%tw
                  "h-8 w-8 rounded-lg ml-2 mr-2 shadow-lg border cursor-pointer"
                ]
                style={ReactDOMRe.Style.make(
                  ~background=GameTypes.nodeColorToColor(v),
                  (),
                )}
                onClick={_ =>
                  handleColorChange(
                    ~color=Some(v),
                    ~activeField,
                    ~setActiveField,
                    ~gameState,
                    ~setGameState,
                    ~setForceUpdate,
                  )
                }
              />;
            })
         |> React.array}
      </div>
    }>
    <div
      className=[%tw
        "h-8 w-8 bg-white rounded-lg ml-8 shadow-lg border cursor-pointer"
      ]
      style={ReactDOMRe.Style.make(
        ~background=
          GameTypes.nodeColorToColor(
            Belt.Option.getWithDefault(color, GameTypes.White),
          ),
        (),
      )}
    />
  </Tippy>;
};
