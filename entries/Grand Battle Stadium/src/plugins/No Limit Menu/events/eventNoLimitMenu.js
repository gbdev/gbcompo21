export const id = "PT_EVENT_NO_LIMIT_MENU";

export const name = "Text: Display Menu (No char limit)";

export const fields = [].concat(
  [
    {
      key: "variable",
      type: "variable",
      defaultValue: "LAST_VARIABLE"
    },
    {
      key: "items",
      label: "Number of options",
      type: "number",
      min: 2,
      max: 8,
      defaultValue: 2
    }
  ],
  Array(8)
    .fill()
    .reduce((arr, _, i) => {
      const value = i + 1;
      arr.push({
        key: `option${i + 1}`,
        label: `Set to '${i + 1}' if`,
        type: "text",
        defaultValue: "",
        placeholder: `${i + 1}`,
        conditions: [
          {
            key: "items",
            gt: value
          }
        ]
      },{
        key: `option${i + 1}`,
        label: `Set to '${i + 1}' if`,
        type: "text",
        defaultValue: "",
        placeholder: `${i + 1}`,
        conditions: [
          {
            key: "items",
            eq: value
          },
          {
            key: "cancelOnLastOption",
            ne: true
          }
        ]
      },{
        key: `option${i + 1}`,
        label: `Set to '0' if`,
        type: "text",
        defaultValue: "",
        placeholder: '0',
        conditions: [
          {
            key: "items",
            eq: value
          },
          {
            key: "cancelOnLastOption",
            eq: true
          }
        ]
      });
      return arr;
    }, []),
    {
      type: "checkbox",
      label: "Last option sets to '0'",
      key: "cancelOnLastOption",
    }, 
    {
      type: "checkbox",
      label: "Set to '0' if B is pressed",
      key: "cancelOnB",
      defaultValue: true
    },
    {
      key: "layout",
      type: "select",
      label: "Layout",
      options: [
        ["dialogue", "Dialogue"],
        ["menu", "Menu"]
      ],
      defaultValue: "dialogue"
    }
);

export const compile = (input, helpers) => {
  const { textMenu } = helpers;
  textMenu(input.variable, [input.option1, input.option2, input.option3, input.option4, input.option5, input.option6, input.option7, input.option8].splice(0, input.items), input.layout, input.cancelOnLastOption, input.cancelOnB);
};
