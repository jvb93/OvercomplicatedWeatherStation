<template>
  <article class="tile is-child notification is-white">
    <p class="title has-text-weight-light">Conditions Over Time</p>
    <div class="content" v-if="dataCollection">
      <line-chart :chart-data="dataCollection" :options="chartOptions" style="max-height: 500px;" />
    </div>
  </article>
</template>

<script>
import LineChart from "@/components/charts/lineChart.js";
import axios from "axios";
import cToF from "@/helpers/TemperatureConverter.js";
import dayjs from "dayjs";
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
      });
    },
  },
};
</script>
