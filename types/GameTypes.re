type state =
  | Builder
  | Player;

type nodeColor =
  | White
  | Black
  | Green;

let nodeColorToColor = nodeColor =>
  switch (nodeColor) {
  | White => "#fff"
  | Black => "#000"
  | Green => "#799351"
  };

type nodeTypes =
  | Color(option(nodeColor));

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

let nodeTypesToData = nodeType =>
  switch (nodeType) {
  | Color(_) => {string: "Color", visual: String("C")}
  };

type testNode = {
  active: bool,
  type_: option(nodeTypes),
};

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
