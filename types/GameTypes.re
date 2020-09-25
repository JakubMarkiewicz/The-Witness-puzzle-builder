type state =
  | Builder
  | Player;

type pathTypes =
  | Start
  | End
  | Hexagon;

type visualType =
  | String(string)
  | Svg(string);
type pathTypeData = {
  string,
  visual: visualType,
};

let pathTypesToData = pathType =>
  switch (pathType) {
  | Start => {string: "Start", visual: String("S")}
  | End => {string: "End", visual: String("E")}
  | Hexagon => {string: "Hexagon", visual: Svg("/static/hexagon.svg")}
  };

type testNode = {active: bool};

type testPath = {
  active: bool,
  type_: option(pathTypes),
};

type node =
  | Path(testPath)
  | Node(testNode);

type activeField = {
  node,
  cords: (int, int),
};
