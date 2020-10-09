<template>
  <article
    class="tile is-parent has-text-centered notification is-white drop-shadow"
    v-if="battery && maxBattery"
  >
    <article class="tile is-5 is-child">
      <b-tooltip :label="batteryPercentage" type="is-dark" position="is-right">
        <h1 class="title is-1 has-text-weight-light">
          {{ battery[battery.length - 1] }}
        </h1>
      </b-tooltip>
      <p class="subtitle">Battery Voltage</p>
    </article>
    <article class="tile is-7 is-child">
      <line-chart
        :chart-data="dataCollection"
        :options="chartOptions"
        style="max-height: 100px"
      />
    </article>
  </article>
</template>

<script>
import axios from "axios";
import dayjs from "dayjs";
import LineChart from "@/components/charts/lineChart.js";
import config from "@/helpers/ConfigProvider.js";
export default {
  components: {
    LineChart,
  },
  data() {
    return {
      dataCollection: null,
      battery: null,
      maxBattery: null,
      minBattery: null,
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
  computed: {
    batteryPercentage() {
      if (this.battery && this.maxBattery && this.minBattery) {
        let currentMillivolts = this.battery[this.battery.length - 1] * 1000;
        let normalizedMax =
          this.maxBattery[0].voltage - this.minBattery[0].voltage;
        let rawPercent = (currentMillivolts / normalizedMax) * 100;
        return `${rawPercent.toFixed(2)}%`;
      }

      return null;
    },
  },
  methods: {
    async getData() {
      let host = config.value("backendHost");
      let fetched = await axios.get(`${host}/battery?limit=288`);
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
            pointRadius: 1,
          },
        ],
      };

      let fetchedMaxBattery = await axios.get(`${host}/maxbattery`);
      this.maxBattery = fetchedMaxBattery.data;
      let fetchedMinBattery = await axios.get(`${host}/minbattery`);
      this.minBattery = fetchedMinBattery.data;
    },
  },
  mounted() {
    this.getData();
  },
};
</script>
