import React from "react";
import Head from "next/head";
import "../tailwind.css";
import "../index.css";
import "tippy.js/dist/tippy.css";
import "tippy.js/themes/light.css";

const App = ({ Component }) => (
  <>
    <Head>
      <title>Test</title>
    </Head>
    <Component apolloClient />
  </>
);

export default App;
