<template>
  <div class="columns has-text-centered" v-if="conditions">
    <div class="column">
      <h1 class="title is-1 has-text-weight-light">{{ conditions.celsius }}</h1>
      <h3 class="subtitle">Temperature</h3>
    </div>
    <div class="column">
      <h1 class="title is-1 has-text-weight-light">
        {{ conditions.relative_humidity }}%
      </h1>
      <h3 class="subtitle">Relative Humidity</h3>
    </div>
  </div>
</template>

<script>
import axios from "axios";
export default {
  data() {
    return {
      conditions: null,
    };
  },
  methods: {
    async getData() {
      let fetched = await axios.get(`${process.env.VUE_APP_API_URL}/weather`);
      this.conditions = fetched.data[0];
    },
  },
  mounted() {
    this.getData();
  },
};
</script>
