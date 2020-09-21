const withTM = require("next-transpile-modules")([
  "bs-platform",
  "bs-webapi",
  "re-classnames",
]);

module.exports = withTM({
  pageExtensions: ["jsx", "js", "bs.js"],
});
