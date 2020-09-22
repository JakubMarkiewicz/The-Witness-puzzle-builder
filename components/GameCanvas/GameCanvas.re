open GameTypes;

module Styles = {
  let canvasStyle = [%tw "bg-gray-900"];
};

external convertTargetToDomElement: Js.t({..}) => Webapi.Dom.Element.t =
  "%identity";

[@react.component]
let make = (~gameState, ~setGameState, ~view, ~activeField, ~setActiveField) => {
  let canvasRef = React.useRef(Js.Nullable.null);

  React.useEffect1(
    _ => {
      switch (React.Ref.current(canvasRef) |> Js.Nullable.toOption) {
      | Some(canvasRef) =>
        GameCanvasUtils.draw(~canvas=canvasRef, ~gameState, ~activeField)
      | _ => ()
      };
      None;
    },
    [|gameState|],
  );
  <canvas
    ref={ReactDOMRe.Ref.domRef(canvasRef)}
    width="600px"
    height="600px"
    className=Styles.canvasStyle
    onClick={ev =>
      switch (view) {
      | Builder =>
        GameCanvasUtils.handleClick(
          ev,
          gameState,
          setGameState,
          ReactEvent.Mouse.target(ev) |> convertTargetToDomElement,
          setActiveField,
        )
      | Player => ()
      }
    }
    onContextMenu={ev => {
      ReactEvent.Mouse.preventDefault(ev);
      GameCanvasUtils.handleContextMenu(
        ~event=ev,
        ~canvas=ReactEvent.Mouse.target(ev) |> convertTargetToDomElement,
        ~gameState,
        ~activeField,
        ~setActiveField,
      );
    }}
  />;
};

let default = make;
