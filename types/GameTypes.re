type state =
  | Builder
  | Player;

type pathTypes =
  | Start
  | End;

type pathTypeData = {
  string,
  visual: string,
};

let pathTypesToData = pathType =>
  switch (pathType) {
  | Start => {string: "Start", visual: "S"}
  | End => {string: "End", visual: "E"}
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
