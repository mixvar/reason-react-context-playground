open Belt;

module State = {
  type t = int;

  let initial: t = 0;

  type action =
    | Increment
    | Decrement
    | Set(t)
    | Reset;

  let reducer = (state: t, action) =>
    switch (action) {
    | Increment => state + 1
    | Decrement => state - 1
    | Set(value) => value
    | Reset => initial
    };

  let useReducer = () => React.useReducer(reducer, initial);
};

module Context = {
  type t = {
    count: State.t,
    dispatch: State.action => unit,
  };

  let reactContext = React.createContext(None);

  module Provider = {
    [@react.component]
    let make = (~children) => {
      let (count, dispatch) = State.useReducer();

      let ctx: option(t) =
        Some({count, dispatch})
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
      let {dispatch}: Context.t = Context.useCount();

      <div>
        <button onClick={_e => dispatch(State.Increment)}>
          {React.string("Increment")}
        </button>
        <button onClick={_e => dispatch(State.Decrement)}>
          {React.string("Decrement")}
        </button>
        <button onClick={_e => dispatch(State.Reset)}>
          {React.string("Reset")}
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
          <h3> {React.string("counter using useReducer and context")} </h3>
          <CountDisplay />
          <Counter />
        </div>
      </Context.Provider>;
    };
  };
};