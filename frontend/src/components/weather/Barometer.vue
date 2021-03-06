<template>
  <article
    class="tile is-parent has-text-centered notification is-white drop-shadow"
    v-if="dataCollection"
  >
    <article class="tile is-5 is-child">
      <h1 class="title is-1 has-text-weight-light">{{ pressures[pressures.length - 1] }}</h1>
      <h3 class="subtitle">{{ units }}</h3>
    </article>
    <article class="tile is-7 is-child">
      <line-chart :chart-data="dataCollection" :options="chartOptions" style="max-height:100px;" />
    </article>
  </article>
</template>

<script>
import LineChart from "@/components/charts/lineChart.js";
import axios from "axios";
import hPaToInHg from "@/helpers/PressureConverter.js";
import dayjs from "dayjs";
import config from "@/helpers/ConfigProvider.js";
import sealevelCompensator from "@/helpers/SealevelCompensator.js";
export default {
  components: {
    LineChart,
  },
  computed: {
    units() {
      return localStorage["useImperial"] == "true" ? "inHg" : "mb";
    },
  },
  data() {
    return {
      dataCollection: null,
      pressures: null,
      chartOptions: {
        responsive: true,
        maintainAspectRatio: false,
        legend: {
          display: false,
        },
        tooltips: {
          callbacks: {
            label: function (tooltipItem, data) {
              var label = data.datasets[tooltipItem.datasetIndex].label || "";

              if (label) {
                label += ": ";
              }
              label += tooltipItem.yLabel.toFixed(2);
              return label;
            },
          },
        },
        scales: {
          yAxes: [
            {
              ticks: {
                display: false,
              },
              gridLines: {
                display: false,
              },
            },
          ],
          xAxes: [
            {
              ticks: {
                display: false,
              },
              gridLines: {
                display: false,
              },
            },
          ],
        },
      },
    };
  },
  mounted() {
    this.getData();
  },
  methods: {
    async getData() {
      let host = config.value("backendHost");
      let fetched = await axios.get(`${host}/weather?limit=288`);
      var chartData = fetched.data.reverse();
      let labels = [];
      let p = [];
      let useImperial = localStorage["useImperial"];

      for (const data of chartData) {
        if (data.pressure == null) {
          continue;
        }

        let compensatedPressure = sealevelCompensator(
          data.pressure,
          219,
          data.celsius
        );

        labels.push(dayjs(data.createdAt).format("h:mm:ss A"));
        if (useImperial == "true") {
          p.push(+hPaToInHg(compensatedPressure).toFixed(2));
        } else {
          p.push(+compensatedPressure.toFixed(2));
        }
      }
      this.pressures = p;
      this.dataCollection = {
        labels: labels,
        datasets: [
          {
            label: "Barometric Pressure",
            backgroundColor: "#556270",
            borderColor: "#556270",
            data: p,
            fill: false,
            pointRadius: 1,
          },
        ],
      };
    },
  },
};
</script>

<style lang="scss" scoped></style>
