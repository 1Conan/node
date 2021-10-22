import module from 'module';

const GET_BUILTIN = `$__get_builtin_hole_${Date.now()}`;

export function globalPreload() {
  return `Object.defineProperty(globalThis, ${JSON.stringify(GET_BUILTIN)}, {
  value: (builtinName) => {
    return getBuiltin(builtinName);
  },
  enumerable: false,
  configurable: false,
});
`;
}

export function resolve(specifier, context, next) {
  const def = next(specifier, context);

  if (def.url.startsWith('node:')) {
    return {
      url: `custom-${def.url}`,
    };
  }
  return def;
}

export function load(url, context, next) {
  if (url.startsWith('custom-node:')) {
    const urlObj = new URL(url);
    return {
      source: generateBuiltinModule(urlObj.pathname),
      format: 'module',
    };
  }
  return next(url, context);
}

function generateBuiltinModule(builtinName) {
  const builtinInstance = module._load(builtinName);
  const builtinExports = [
    ...Object.keys(builtinInstance),
  ];
  return `\
const $builtinInstance = ${GET_BUILTIN}(${JSON.stringify(builtinName)});

export const __fromLoader = true;

export default $builtinInstance;

${
  builtinExports
    .map(name => `export const ${name} = $builtinInstance.${name};`)
    .join('\n')
}
`;
}
