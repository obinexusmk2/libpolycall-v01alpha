import React from "react";

export default function CIRVisualizer({ data }) {
  return <pre>{JSON.stringify(data, null, 2)}</pre>;
}
