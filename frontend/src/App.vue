<template>
  <div class="container is-fullhd">
    <b-switch v-model="useImperial" size="is-small" class="has-text-light">imperial</b-switch>
    <div class="tile is-ancestor mt-6">
      <div class="tile is-vertical is-10">
        <div class="tile">
          <div class="tile is-parent is-vertical">
            <current-conditions />
          </div>
          <div class="tile is-parent is-vertical">
            <battery />
            <barometer />
          </div>
        </div>
        <div class="tile is-parent">
          <weather-chart />
        </div>
      </div>
    </div>
    <span id="background" :class="backgroundClass"></span>
  </div>
</template>

<script>
import CurrentConditions from "@/components/weather/CurrentConditions.vue";
import WeatherChart from "@/components/weather/WeatherChart.vue";
import Battery from "@/components/device/Battery.vue";
import Barometer from "@/components/weather/Barometer.vue";
export default {
  name: "App",
  components: { CurrentConditions, Battery, WeatherChart, Barometer },
  computed: {
    backgroundClass() {
      let now = new Date();
      let hour = now.getHours();
      if (hour > 20) {
        return "nighttime";
      }
      return "daytime";
    },
    useImperial: {
      // getter
      get: function () {
        return localStorage["useImperial"];
      },
      // setter
      set: function (newValue) {
        if (newValue) {
          localStorage["useImperial"] = true;
        } else {
          localStorage["useImperial"] = false;
        }

        location.reload();
      },
    },
  },
};
</script>

<style>
#background {
  z-index: -1;
  position: fixed;
  top: 0px;
  left: 0px;
  min-width: 100vw;
  min-height: 100vh;
}

#background.nighttime {
  background-color: #000000;
  background-image: linear-gradient(147deg, #000000 0%, #2c3e50 74%);
}

#background.daytime {
  background-color: #2a2a72;
  background-image: linear-gradient(315deg, #2a2a72 0%, #009ffd 74%);
}
</style>
