module App = {
  [@react.component]
  let make = () =>
    <>
      <SimpleCounter.Components.CountPage />
      <CounterWithContext.Components.CountPage />
      <CounterWithContextAndReducer.Components.CountPage />
    </>;
};

ReactDOMRe.renderToElementWithId(<App />, "app");