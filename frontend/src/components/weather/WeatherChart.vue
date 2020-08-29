<template>
  <article class="tile is-parent notification is-vertical is-white drop-shadow">
    <article class="tile is-child" v-if="dataCollection">
      <line-chart
        :chart-data="dataCollection"
        :options="chartOptions"
        style="max-height: 500px;"
      />
    </article>
  </article>
</template>

<script>
import LineChart from "@/components/charts/lineChart.js";
import axios from "axios";
import cToF from "@/helpers/TemperatureConverter.js";
import dayjs from "dayjs";
import config from "@/helpers/ConfigProvider.js";

export default {
  components: {
    LineChart,
  },
  data() {
    return {
      dataCollection: null,
      chartOptions: {
        responsive: true,
        maintainAspectRatio: false,
        tooltips: {
          callbacks: {
            label: function(tooltipItem, data) {
              var label = data.datasets[tooltipItem.datasetIndex].label || "";

              if (label) {
                label += ": ";
              }
              label += tooltipItem.yLabel.toFixed(2);
              return label;
            },
          },
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
      let fetched = await axios.get(`${host}/weather`);
      var chartData = fetched.data.reverse();

      let labels = [];
      let temperatures = [];
      let humidity = [];
      let useImperial = localStorage["useImperial"];

      for (const data of chartData) {
        labels.push(dayjs(data.createdAt).format("h:mm:ss A"));
        if (useImperial == "true") {
          temperatures.push(cToF(data.celsius));
        } else {
          temperatures.push(data.celsius);
        }
        humidity.push(data.relative_humidity);
      }
      this.dataCollection = {
        labels: labels,
        datasets: [
          {
            label: "Temperature",
            backgroundColor: "#F38630",
            borderColor: "#F38630",
            data: temperatures,
            fill: false,
          },
          {
            label: "Humidity",
            backgroundColor: "#A7DBD8",
            borderColor: "#A7DBD8",
            data: humidity,
            fill: false,
          },
        ],
      };
    },
  },
};
</script>
