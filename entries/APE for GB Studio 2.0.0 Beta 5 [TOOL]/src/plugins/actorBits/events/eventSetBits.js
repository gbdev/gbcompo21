export const id = "ACTOR_SET_BITS";
export const name = "Actor: Set Bits";
export const fields = [
  {
    key: "actorId",
    label: "Actor",
    type: "actor",
    defaultValue: "$self$"
  },
  {
    key: "cpal",
    label: "Palette Index (GBC)",
    type: "union",
    types: ["number", "variable", "property"],
    defaultType: "number",
    width: "50%",
    min: 0,
    max: 7,
    defaultValue: {
      number: 0,
      variable: "LAST_VARIABLE",
      property: "$self$:xpos"
    }
  },
  {
    key: "pal",
    label: "Palette (DMG)",
    type: "select",
    width: "50%",
    options: [
      [0, "OBP0"],
      [1, "OBP1"]
    ],
    defaultValue: 0
  },
  {
    key: "flipX",
    label: "Horizontal Flip",
    type: "checkbox",
    width: "50%",
    defaultValue: false
  },
  {
    key: "flipY",
    label: "Vertical Flip",
    type: "checkbox",
    width: "50%",
    defaultValue: false
  },
  {
    key: "bank",
    label: "Tile VRAM Bank (GBC)",
    type: "select",
    width: "50%",
    options: [
      [0, "Bank 0"],
      [1, "Bank 1"]
    ],
    defaultValue: 0
  },
  {
    key: "priority",
    label: "BG Priority",
    type: "checkbox",
    width: "50%",
    defaultValue: false
  }
];
export const compile = (input, helpers) => {
  const { variablesAdd, variableCopy, variableSetToValue, engineFieldSetToValue, engineFieldSetToVariable, variableFromUnion, temporaryEntityVariable, actorSetFlip, actorSetActive } = helpers;
  const {actorId, cpal, pal, flipX, flipY, bank, priority} = input;
  actorSetActive(actorId);
  if (cpal.type == "number") {
    engineFieldSetToValue("bits", 
      cpal.value +
      bank * 8 +
      pal * 16 +
      flipX * 32 +
      flipY * 64 +
      priority * 128
    );
  } else {
    const cp = variableFromUnion(cpal, temporaryEntityVariable(0));
    const cp2 = "cp2";
    variableCopy(cp2, cp);
    const tmp1 = "tmp1";
    variableSetToValue(tmp1,
      bank * 8 +
      pal * 16 +
      flipX * 32 +
      flipY * 64 +
      priority * 128
    );
    variablesAdd(cp2, tmp1);
    engineFieldSetToVariable("bits", cp2);
  }
  engineFieldSetToValue("actor_i", 0);
  actorSetFlip(1);
};