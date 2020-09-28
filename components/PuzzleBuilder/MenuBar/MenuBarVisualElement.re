[@react.component]
let make = (~visual: GameTypes.visualType) =>
  switch (visual) {
  | String(v) => React.string(v)
  | Svg(svgPath) => <img src=svgPath />
  };
