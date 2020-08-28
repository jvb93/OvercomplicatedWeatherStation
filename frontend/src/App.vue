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

      if (hour < 5) {
        return "night";
      } else if (hour >= 5 && hour <= 7) {
        return "dawn";
      } else if (hour > 7 && hour <= 17) {
        return "day";
      } else if (hour > 17 && hour < 20) {
        return "dusk";
      }
      return "day";
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

#background.dawn {
  background-image: linear-gradient(315deg, #fce043 0%, #fb7ba2 74%);
  background-size: 400% 400%;
  -webkit-animation: gradientBG 20s ease infinite;
  animation: gradientBG 20s ease infinite;
}

#background.dusk {
  background-image: linear-gradient(315deg, #d4418e 0%, #0652c5 74%);
  background-size: 400% 400%;
  -webkit-animation: gradientBG 20s ease infinite;
  animation: gradientBG 20s ease infinite;
}

#background.night {
  background: linear-gradient(147deg, #000000 0%, #2c3e50 74%);
  background-size: 400% 400%;
  -webkit-animation: gradientBG 20s ease infinite;
  animation: gradientBG 20s ease infinite;
}

#background.day {
  background: linear-gradient(315deg, #2a2a72 0%, #009ffd 74%);
  background-size: 400% 400%;
  -webkit-animation: gradientBG 20s ease infinite;
  animation: gradientBG 20s ease infinite;
}
@-webkit-keyframes gradientBG {
  0% {
    background-position: 0% 50%;
  }
  50% {
    background-position: 100% 50%;
  }
  100% {
    background-position: 0% 50%;
  }
}
@keyframes gradientBG {
  0% {
    background-position: 0% 50%;
  }
  50% {
    background-position: 100% 50%;
  }
  100% {
    background-position: 0% 50%;
  }
}
</style>
