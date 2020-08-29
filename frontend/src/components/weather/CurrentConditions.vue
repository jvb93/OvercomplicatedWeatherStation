<template>
  <article class="tile is-parent notification is-vertical is-white drop-shadow">
    <article class="tile is-parent has-text-centered" v-if="conditions">
      <article class="tile is-child">
        <h1 class="title is-1 has-text-weight-light">
          {{ Math.round(conditions.celsius) }} {{ units }}
        </h1>
        <h3 class="subtitle">Temperature</h3>
      </article>
      <article class="tile is-child">
        <h1 class="title is-1 has-text-weight-light">
          {{ Math.round(conditions.relative_humidity) }}%
        </h1>
        <h3 class="subtitle">Relative Humidity</h3>
      </article>
    </article>
    <article class="tile is-child bottom-text" v-if="conditions">
      <p class="is-size-7">
        Last Update: {{ formatDateTime(conditions.createdAt) }}
      </p>
    </article>
  </article>
</template>

<script>
import axios from "axios";
import cToF from "@/helpers/TemperatureConverter.js";
import dayjs from "dayjs";
import config from "@/helpers/ConfigProvider.js";

export default {
  data() {
    return {
      conditions: null,
    };
  },
  computed: {
    units() {
      return localStorage["useImperial"] == "true" ? "°F" : "°C";
    },
  },
  methods: {
    async getData() {
      let host = config.value("backendHost");
      let fetched = await axios.get(`${host}/weather`);
      this.conditions = this.convertImperialIfSpecified(fetched.data[0]);
    },
    convertImperialIfSpecified(conditions) {
      if (localStorage["useImperial"] == "true") {
        conditions.celsius = cToF(conditions.celsius);
      }

      return conditions;
    },
    formatDateTime(time) {
      return dayjs(time).format("MMMM D, YYYY h:mm A");
    },
  },
  mounted() {
    this.getData();
  },
};
</script>

<style scoped>
.bottom-text {
  display: flex;
}

.bottom-text p {
  align-self: flex-end;
}
</style>
