<template>
  <article class="tile is-child notification is-white" v-if="battery">
    <h1 class="title is-1 has-text-weight-light">{{ battery.voltage }}</h1>
    <p class="subtitle">Battery Voltage</p>
  </article>
</template>

<script>
import axios from "axios";
export default {
  data() {
    return {
      battery: null,
    };
  },
  methods: {
    async getData() {
      let fetched = await axios.get(`${process.env.VUE_APP_API_URL}/battery`);
      this.battery = fetched.data[0];
      this.battery.voltage = (this.battery.voltage / 1000).toFixed(3);
    },
  },
  mounted() {
    this.getData();
  },
};
</script>
