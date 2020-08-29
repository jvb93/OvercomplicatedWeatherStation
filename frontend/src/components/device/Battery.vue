<template>
  <article class="tile is-parent has-text-centered notification is-white" v-if="battery">
    <article class="tile is-5 is-child">
      <h1 class="title is-1 has-text-weight-light">{{ battery[battery.length-1]}}</h1>
      <p class="subtitle">Battery Voltage</p>
    </article>
    <article class="tile is-7 is-child">
      <line-chart :chart-data="dataCollection" :options="chartOptions" style="max-height:100px;" />
    </article>
  </article>
</template>

<script>
import axios from "axios";
import dayjs from "dayjs";
import LineChart from "@/components/charts/lineChart.js";

export default {
  components: {
    LineChart,
  },
  data() {
    return {
      dataCollection: null,
      battery: null,
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

  methods: {
    async getData() {
      let fetched = await axios.get(
        `${process.env.VUE_APP_API_URL}/battery?limit=100`
      );
      var chartData = fetched.data.reverse();
      let labels = [];
      let p = [];
      for (const data of chartData) {
        labels.push(dayjs(data.createdAt).format("h:mm:ss A"));
        p.push(+(data.voltage / 1000).toFixed(3));
      }

      this.battery = p;
      this.dataCollection = {
        labels: labels,
        datasets: [
          {
            label: "Volts",
            backgroundColor: "#556270",
            borderColor: "#556270",
            data: p,
            fill: false,
          },
        ],
      };
    },
  },
  mounted() {
    this.getData();
  },
};
</script>
