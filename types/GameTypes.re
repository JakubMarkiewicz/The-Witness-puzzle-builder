type state =
  | Builder
  | Player;

type testNode = {active: bool};

type testPath = {active: bool};

type node =
  | Path(testPath)
  | Node(testNode);

type activeField = {
  node,
  cords: (int, int),
};
