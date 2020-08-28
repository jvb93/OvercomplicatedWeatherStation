<template>
  <article class="tile is-parent has-text-centered notification is-white" v-if="dataCollection">
    <article class="tile is-4 is-child">
      <h1 class="title is-1 has-text-weight-light">{{pressures[0].toFixed(2)}}</h1>
      <h3 class="subtitle">{{units}}</h3>
    </article>
    <article class="tile is-8 is-child">
      <line-chart :chart-data="dataCollection" :options="chartOptions" style="max-height:100px;" />
    </article>
  </article>
</template>

<script>
import LineChart from "@/components/charts/lineChart.js";
import axios from "axios";
import paToInHg from "@/helpers/PressureConverter.js";
import dayjs from "dayjs";
export default {
  components: {
    LineChart,
  },
  computed: {
    units() {
      return localStorage["useImperial"] == "true" ? "mmHg" : "mb";
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
    getData() {
      axios.get(`${process.env.VUE_APP_API_URL}/weather`).then((resp) => {
        var chartData = resp.data.reverse();
        let labels = [];
        let p = [];
        let useImperial = localStorage["useImperial"];

        for (const data of chartData) {
          if (data.pressure == null) {
            continue;
          }
          labels.push(dayjs(data.createdAt).format("h:mm:ss A"));
          if (useImperial == "true") {
            p.push(paToInHg(data.pressure));
          } else {
            p.push(data.pressure / 100);
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
            },
          ],
        };
      });
    },
  },
};
</script>

<style lang="scss" scoped>
</style>