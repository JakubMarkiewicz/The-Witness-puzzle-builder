[@react.component]
let make = (~gameState) => {
  let (painting, setPainting) = React.useState(_ => false);
  let (path, setPath) = React.useState(_ => [||]);
  <canvas
    width="600px"
    height="600px"
    className=[%tw "absolute mt-4 top-0"]
    onMouseDown={ev =>
      GameCanvasPlayerUtils.handleMouseDown(~ev, ~gameState, ~setPainting)
    }
    onMouseMove={ev =>
      GameCanvasPlayerUtils.handleOnMouseMove(
        ~ev,
        ~gameState,
        ~painting,
        ~setPath,
        ~path,
      )
    }
    onMouseUp={ev =>
      GameCanvasPlayerUtils.handleOnMouseUp(
        ~ev,
        ~gameState,
        ~setPainting,
        ~setPath,
        ~path,
      )
    }
  />;
};
