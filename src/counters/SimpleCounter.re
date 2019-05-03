open Belt;

module Components = {
  module Counter = {
    [@react.component]
    let make = (~onIncrement, ~onDecrement) =>
      <div>
        <button onClick={_e => onIncrement()}>
          {React.string("Increment")}
        </button>
        <button onClick={_e => onDecrement()}>
          {React.string("Decrement")}
        </button>
      </div>;
  };

  module CountDisplay = {
    [@react.component]
    let make = (~count) =>
      <div> {React.string("Count: " ++ Int.toString(count))} </div>;
  };

  module CountPage = {
    let containerStyle =
      ReactDOMRe.Style.make(~margin="100px auto", ~maxWidth="400px", ());

    [@react.component]
    let make = () => {
      let (count, setCount) = React.useState(() => 0);

      <div style=containerStyle>
        <h3> {React.string("simple counter using useState hook")} </h3>
        <CountDisplay count />
        <Counter
          onIncrement={() => setCount(c => c + 1)}
          onDecrement={() => setCount(c => c - 1)}
        />
      </div>;
    };
  };
};