export const id = "ACTOR_MOVE_AWAY";
export const name = "Actor: Move Away From Position";

export const fields = [
  {
    key: "actorId",
    label: "Actor",
    type: "actor",
    defaultValue: "$self$",
  },
  {
    key: "x",
    label: "X",
    type: "union",
    types: ["number", "variable", "property"],
    defaultType: "number",
    min: 0,
    max: 255,
    width: "50%",
    defaultValue: {
      number: 0,
      variable: "LAST_VARIABLE",
      property: "$self$:xpos",
    },
  },
  {
    key: "y",
    label: "Y",
    type: "union",
    types: ["number", "variable", "property"],
    defaultType: "number",
    min: 0,
    max: 255,
    width: "50%",
    defaultValue: {
      number: 0,
      variable: "LAST_VARIABLE",
      property: "$self$:xpos",
    },
  },
  {
    key: "moveType",
    label: "Movement Type",
    type: "select",
    options: [
      ["horizontal", "↔ " + "Horizontal"],
      ["vertical", "↕ " + "Vertical"],
      ["diagonal", "⤡ " + "Diagonal"],
    ],
    defaultValue: "horizontal",
    width: "50%",
  },
  {
    key: "distance",
    label: "Distance",
    type: "union",
    types: ["number", "variable", "property"],
    defaultType: "number",
    min: 0,
    max: 255,
    width: "50%",
    defaultValue: {
      number: 1,
      variable: "LAST_VARIABLE",
      property: "$self$:xpos",
    },
  },
  {
    key: "useCollisions",
    label: "Use Collisions",
    type: "checkbox",
    defaultValue: false,
    width: "50%"
  },
];

export const compile = (input, helpers) => {
  const { actorSetActive, actorMoveToVariables, variableFromUnion, temporaryEntityVariable, actorGetPosition, ifVariableCompare, variableSetToValue, variablesAdd, variablesSub, variableCopy, ifActorDirection } = helpers;
  const {actorId, x, y, moveType, distance, useCollisions} = input;
  actorSetActive(actorId);
  const actorX = "actorX";
  const actorY = "actorY";
  const destX = "destX";
  const destY = "destY";
  const dist = "dist";
  var xVar = "xVar";
  var yVar = "yVar";
  var distanceVar = "distanceVar";
  if (x.type === "number" && y.type === "number" && distance.type === "number") {
    variableSetToValue(xVar, x.value);
    variableSetToValue(yVar, y.value);
    variableSetToValue(distanceVar, distance.value);
  } else {
    xVar = variableFromUnion(x, temporaryEntityVariable(0));
    yVar = variableFromUnion(y, temporaryEntityVariable(1));
    distanceVar = variableFromUnion(distance, temporaryEntityVariable(2));
  }
  actorGetPosition(actorX, actorY);
  variableCopy(dist, distanceVar);
  variableCopy(destX, actorX);
  variableCopy(destY, actorY);
  ifVariableCompare(actorX, "<", xVar, ()=>{variablesSub(destX, dist, true);}, ()=>{ifVariableCompare(actorX, ">", xVar, ()=>{variablesAdd(destX, dist, true);}, []);});
  ifVariableCompare(actorY, "<", yVar, ()=>{variablesSub(destY, dist, true);}, ()=>{ifVariableCompare(actorY, ">", yVar, ()=>{variablesAdd(destY, dist, true);}, []);});
  ifVariableCompare(destX, "==", actorX, ()=>{
    ifVariableCompare(destY, "==", actorY, ()=>{
      ifActorDirection("down", ()=>{variablesSub(destY, dist, true);}, []);
      ifActorDirection("right", ()=>{variablesSub(destX, dist, true);}, []);
      ifActorDirection("up", ()=>{variablesAdd(destY, dist, true);}, []);
      ifActorDirection("left", ()=>{variablesAdd(destX, dist, true);}, []);
      actorMoveToVariables(destX, destY, useCollisions, moveType);
      actorGetPosition(actorX, actorY);
      variableCopy(destX, actorX);
      variableCopy(destY, actorY);
    }, []);
  }, []);
  switch (moveType) {
    case "horizontal":
      actorMoveToVariables(destX, actorY, useCollisions, moveType);
      break;
    case "vertical":
      actorMoveToVariables(actorX, destY, useCollisions, moveType);
      break;
    case "diagonal":
      actorMoveToVariables(destX, destY, useCollisions, moveType);
      break;
  }
};