export const id = "ACTOR_MOVE_TOWARDS";
export const name = "Actor: Move Towards Position";

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
      ["horizontal", "↔ " + "Horizontal First"],
      ["vertical", "↕ " + "Vertical First"],
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
  switch (moveType) {
    case "horizontal":
        ifVariableCompare(actorX, "<", xVar, ()=>{
          variablesAdd(destX, dist, true);
          ifVariableCompare(destX, ">", xVar, ()=>{
            variablesSub(destX, xVar, true);
            ifVariableCompare(actorY, "<", yVar, ()=>{
              variablesAdd(destY, destX, true);
              ifVariableCompare(destY, ">", yVar, ()=>{variableCopy(destY, yVar);});
            }, ()=>{
              ifVariableCompare(actorY, ">", yVar, ()=>{
                variablesSub(destY, destX, true);
                ifVariableCompare(destY, "<", yVar, ()=>{variableCopy(destY, yVar);});
              });
            });
            variableCopy(destX, xVar);
          });
        }, ()=>{
          ifVariableCompare(actorX, ">", xVar, ()=>{
            variablesSub(destX, dist, true);
            ifVariableCompare(destX, "<", xVar, ()=>{
              variableCopy(dist, xVar);
              variablesSub(dist, destX, true);
              ifVariableCompare(actorY, "<", yVar, ()=>{
                variablesAdd(destY, dist, true);
                ifVariableCompare(destY, ">", yVar, ()=>{variableCopy(destY, yVar);});
              }, ()=>{
                ifVariableCompare(actorY, ">", yVar, ()=>{
                  variablesSub(destY, dist, true);
                  ifVariableCompare(destY, "<", yVar, ()=>{variableCopy(destY, yVar);});
                });
              });
              variableCopy(destX, xVar);
            });
          }, ()=>{
            ifVariableCompare(actorY, "<", yVar, ()=>{
              variablesAdd(destY, dist, true);
              ifVariableCompare(destY, ">", yVar, ()=>{variableCopy(destY, yVar);});
            }, ()=>{
              ifVariableCompare(actorY, ">", yVar, ()=>{
                variablesSub(destY, dist, true);
                ifVariableCompare(destY, "<", yVar, ()=>{variableCopy(destY, yVar);});
              }, []);
            });
          });
        });
      break;
    case "vertical":
      ifVariableCompare(actorY, "<", yVar, ()=>{
        variablesAdd(destY, dist, true);
        ifVariableCompare(destY, ">", yVar, ()=>{
          variablesSub(destY, yVar, true);
          ifVariableCompare(actorX, "<", xVar, ()=>{
            variablesAdd(destX, destY, true);
            ifVariableCompare(destX, ">", xVar, ()=>{variableCopy(destX, xVar);});
          }, ()=>{
            ifVariableCompare(actorX, ">", xVar, ()=>{
              variablesSub(destX, destY, true);
              ifVariableCompare(destX, "<", xVar, ()=>{variableCopy(destX, xVar);});
            });
          });
          variableCopy(destY, yVar);
        });
      }, ()=>{
        ifVariableCompare(actorY, ">", yVar, ()=>{
          variablesSub(destY, dist, true);
          ifVariableCompare(destY, "<", yVar, ()=>{
            variableCopy(dist, yVar);
            variablesSub(dist, destY, true);
            ifVariableCompare(actorX, "<", xVar, ()=>{
              variablesAdd(destX, dist, true);
              ifVariableCompare(destX, ">", xVar, ()=>{variableCopy(destX, xVar);});
            }, ()=>{
              ifVariableCompare(actorX, ">", xVar, ()=>{
                variablesSub(destX, dist, true);
                ifVariableCompare(destX, "<", xVar, ()=>{variableCopy(destX, xVar);});
              });
            });
            variableCopy(destY, yVar);
          });
        }, ()=>{
          ifVariableCompare(actorX, "<", xVar, ()=>{
            variablesAdd(destX, dist, true);
            ifVariableCompare(destX, ">", xVar, ()=>{variableCopy(destX, xVar);});
          }, ()=>{
            ifVariableCompare(actorX, ">", xVar, ()=>{
              variablesSub(destX, dist, true);
              ifVariableCompare(destX, "<", xVar, ()=>{variableCopy(destX, xVar);});
            }, []);
          });
        });
      });
      break;
    case "diagonal":
        ifVariableCompare(actorX, "<", xVar, ()=>{
          variablesAdd(destX, dist, true);
          ifVariableCompare(destX, ">", xVar, ()=>{variableCopy(destX, xVar);});
        }, ()=>{
          ifVariableCompare(actorX, ">", xVar, ()=>{
            variablesSub(destX, dist, true);
            ifVariableCompare(destX, "<", xVar, ()=>{variableCopy(destX, xVar);});
          }, []);
        });
        ifVariableCompare(actorY, "<", yVar, ()=>{
          variablesAdd(destY, dist, true);
          ifVariableCompare(destY, ">", yVar, ()=>{variableCopy(destY, yVar);});
        }, ()=>{
          ifVariableCompare(actorY, ">", yVar, ()=>{
            variablesSub(destY, dist, true);
            ifVariableCompare(destY, "<", yVar, ()=>{variableCopy(destY, yVar);});
          }, []);
        });
      break;
  }
  actorMoveToVariables(destX, destY, useCollisions, moveType);
};