export default function compensate(pascals, meters, celsius) {
  return (
    (pascals / 100) *
    Math.pow(
      1 - (0.0065 * meters) / (celsius + 0.0065 * meters + 273.15),
      -5.257
    )
  );
}
