
module.exports = {
    plugins:
    {
        tailwindcss: {},
        autoprefixer: {},
        "@fullhuman/postcss-purgecss": process.env.NODE_ENV === 'production' ? {
            content: ['./**/*.re', './**/*.js'],

            extractors: [
                {
                    extractor: require('@dylanirlbeck/tailwind-ppx').extractor,
                    extensions: ['re'],
                },
            ],
        } : false
    }

}
