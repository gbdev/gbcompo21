export const id = "ACTOR_SET_BIT";
export const name = "Actor: Set Bit";
export const fields = [
  {
    key: "actorId",
    label: "Actor",
    type: "actor",
    defaultValue: "$self$",
    width: "50%"
  },
  {
    key: "bit",
    label: "Bit",
    type: "select",
    width: "50%",
    options: [
      ["0", "Palette Index (GBC)"],
      [4, "Palette (DMG)"],
      [5, "Horizontal Flip"],
      [6, "Vertical Flip"],
      [3, "Tile VRAM Bank (GBC)"],
      [7, "BG Priority"]
    ],
    defaultValue: "0"
  },
  {
    key: "cpal",
    type: "union",
    types: ["number", "variable", "property"],
    defaultType: "number",
    min: 0,
    max: 7,
    defaultValue: {
      number: 0,
      variable: "LAST_VARIABLE",
      property: "$self$:xpos"
    },
    conditions: [
      {
        key: "bit",
        eq: "0"
      }
    ]
  },
  {
    key: "pal",
    type: "select",
    options: [
      [0, "OBP0"],
      [1, "OBP1"]
    ],
    defaultValue: 0,
    conditions: [
      {
        key: "bit",
        eq: 4
      }
    ]
  },
  {
    key: "flipX",
    label: "Horizontal Flip",
    type: "checkbox",
    defaultValue: false,
    conditions: [
      {
        key: "bit",
        eq: 5
      }
    ]
  },
  {
    key: "flipY",
    label: "Vertical Flip",
    type: "checkbox",
    defaultValue: false,
    conditions: [
      {
        key: "bit",
        eq: 6
      }
    ]
  },
  {
    key: "bank",
    type: "select",
    options: [
      [0, "Bank 0"],
      [1, "Bank 1"]
    ],
    defaultValue: 0,
    conditions: [
      {
        key: "bit",
        eq: 3
      }
    ]
  },
  {
    key: "priority",
    label: "BG Priority",
    type: "checkbox",
    defaultValue: false,
    conditions: [
      {
        key: "bit",
        eq: 7
      }
    ]
  }
];
export const compile = (input, helpers) => {
  const { variablesMul, variableCopy, variableSetToValue, engineFieldSetToValue, engineFieldSetToVariable, variableFromUnion, temporaryEntityVariable, actorSetActive, actorSetFlip } = helpers;
  const {actorId, bit, cpal, pal, flipX, flipY, bank, priority} = input;
  actorSetActive(actorId);
  switch (bit) {
    case "0":
      if (cpal.type == "number") {
        engineFieldSetToValue("bits", cpal.value * 16);
      } else {
        const cp = variableFromUnion(cpal, temporaryEntityVariable(0));
        const cp2 = "cp2";
        variableCopy(cp2, cp);
        const tmp1 = "tmp1";
        variableSetToValue(tmp1, 16);
        variablesMul(cp2, tmp1);
        engineFieldSetToVariable("bits", cp2);
      }
      break;
    case 4:
      engineFieldSetToValue("bits", 4 + pal * 16);
      break;
    case 5:
      engineFieldSetToValue("bits", 5 + flipX * 16);
      break;
    case 6:
      engineFieldSetToValue("bits", 6 + flipY * 16);
      break;
    case 3:
      engineFieldSetToValue("bits", 3 + bank * 16);
      break;
    case 7:
      engineFieldSetToValue("bits", 7 + priority * 16);
      break;
  }
  engineFieldSetToValue("actor_i", 1);
  actorSetFlip(1);
};