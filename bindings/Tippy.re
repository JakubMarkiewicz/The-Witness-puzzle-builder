[@bs.module "@tippyjs/react"] [@react.component]
external make:
  (
    ~content: React.element=?,
    ~id: string=?,
    ~placement: string=?,
    ~trigger: [@bs.string] [ | `click | `focusin]=?,
    ~onTrigger: unit => unit=?,
    ~onHide: unit => unit=?,
    ~onShow: unit => unit=?,
    ~className: string=?,
    ~arrow: bool=?,
    ~visible: bool=?,
    ~interactive: bool=?,
    ~ref: React.Ref.t(Js.Nullable.t('a))=?,
    ~appendTo: unit => Dom.element=?,
    ~theme: string=?,
    ~children: React.element
  ) =>
  React.element =
  "default";
