<template>
  <article class="tile is-child notification is-white" v-if="battery">
    <p class="title">{{ battery.voltage / 1000 }}</p>
    <p class="subtitle">Battery Voltage</p>
    <p class="is-size-7">Last Update: {{ battery.createdAt }}</p>
  </article>
</template>

<script>
import axios from "axios";
import dayjs from "dayjs";
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
      this.battery.createdAt = dayjs(this.battery.createdAt).format(
        "MMMM D, YYYY h:mm A"
      );
    },
  },
  mounted() {
    this.getData();
  },
};
</script>
