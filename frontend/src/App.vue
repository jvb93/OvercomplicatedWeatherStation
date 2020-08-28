<template>
  <div class="container is-fullhd">
    <div class="tile is-ancestor mt-6">
      <div class="tile is-vertical is-8">
        <div class="tile">
          <div class="tile is-parent is-vertical">
            <current-conditions />
          </div>
          <div class="tile is-parent is-vertical">
            <battery />
            <article class="tile is-child notification is-white">
              <p class="title">...tiles</p>
              <p class="subtitle">Bottom tile</p>
            </article>
          </div>
        </div>
        <div class="tile is-parent">
          <article class="tile is-child notification is-white">
            <p class="title">Wide tile</p>
            <p class="subtitle">Aligned with the right tile</p>
            <div class="content">
              <!-- Content -->
            </div>
          </article>
        </div>
      </div>
    </div>
    <div class="card"></div>
    <span id="background" :class="backgroundClass"></span>
  </div>
</template>

<script>
import CurrentConditions from "@/components/weather/CurrentConditions.vue";
import Battery from "@/components/device/Battery.vue";
export default {
  name: "App",
  components: { CurrentConditions, Battery },
  computed: {
    backgroundClass() {
      let now = new Date();
      let hour = now.getHours();
      if (hour > 20) {
        return "nighttime";
      }
      return "daytime";
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
