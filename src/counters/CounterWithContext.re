open Belt;

module Context = {
  type t = {
    count: int,
    increment: unit => unit,
    decrement: unit => unit,
  };

  let reactContext = React.createContext(None);

  module Provider = {
    [@react.component]
    let make = (~children) => {
      let (count, setCount) = React.useState(() => 0);

      let ctx: option(t) =
        Some({
          count,
          increment: () => setCount(c => c + 1),
          decrement: () => setCount(c => c - 1),
        })
        |> (it => React.useMemo1(() => it, [|count|]));

      Utils.reactContextProvider(
        ~children,
        ~context=reactContext,
        ~value=ctx,
      );
    };
  };

  exception CountContextNotFound;

  let useCount = () => {
    switch (React.useContext(reactContext)) {
    | Some(ctx) => ctx
    | None => raise(CountContextNotFound)
    };
  };
};

module Components = {
  module Counter = {
    [@react.component]
    let make = () => {
      let {increment, decrement}: Context.t = Context.useCount();

      <div>
        <button onClick={_e => increment()}>
          {React.string("Increment")}
        </button>
        <button onClick={_e => decrement()}>
          {React.string("Decrement")}
        </button>
      </div>;
    };
  };

  module CountDisplay = {
    [@react.component]
    let make = () => {
      let {count}: Context.t = Context.useCount();

      <div> {React.string("Count: " ++ Int.toString(count))} </div>;
    };
  };

  module CountPage = {
    let containerStyle =
      ReactDOMRe.Style.make(~margin="100px auto", ~maxWidth="400px", ());

    [@react.component]
    let make = () => {
      <Context.Provider>
        <div style=containerStyle>
          <h3> {React.string("counter using useState and context")} </h3>
          <CountDisplay />
          <Counter />
        </div>
      </Context.Provider>;
    };
  };
};