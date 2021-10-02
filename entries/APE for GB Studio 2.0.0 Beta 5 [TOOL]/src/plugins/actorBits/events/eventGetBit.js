export const id = "ACTOR_GET_BIT";
export const name = "Actor: Store Bit In Engine Field";
export const fields = [
  {
    label: "Note: The bit will be stored in the 'Actor Bits' engine field."
  },
  {
    key: "actorId",
    label: "Actor Index",
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
      [0, "Palette Index (GBC)"],
      [4, "Palette (DMG)"],
      [5, "Horizontal Flip"],
      [6, "Vertical Flip"],
      [3, "Tile VRAM Bank (GBC)"],
      [7, "BG Priority"]
    ],
    defaultValue: 0
  },
];
export const compile = (input, helpers) => {
  const { engineFieldSetToValue, actorSetActive, actorSetFlip } = helpers;
  const {actorId, bit} = input;
  actorSetActive(actorId);
  engineFieldSetToValue("bits", bit);
  engineFieldSetToValue("actor_i", 2);
  actorSetFlip(1);
};