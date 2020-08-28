<template>
  <div class="container is-fullhd">
    <div class="tile is-ancestor mt-6">
      <div class="tile is-vertical is-8">
        <div class="tile">
          <div class="tile is-parent is-vertical">
            <article class="tile is-child notification is-white">
              <current-conditions />
            </article>
            <article class="tile is-child notification is-warning">
              <p class="title">...tiles</p>
              <p class="subtitle">Bottom tile</p>
            </article>
          </div>
          <div class="tile is-parent">
            <article class="tile is-child notification is-info">
              <p class="title">Middle tile</p>
              <p class="subtitle">With an image</p>
              <figure class="image is-4by3">
                <img src="https://bulma.io/images/placeholders/640x480.png" />
              </figure>
            </article>
          </div>
        </div>
        <div class="tile is-parent">
          <article class="tile is-child notification is-danger">
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
export default {
  name: "App",
  components: { CurrentConditions },
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
  background-image: linear-gradient(147deg, #000000 0%, #434343 74%);
}

#background.daytime {
  background-color: #2a2a72;
  background-image: linear-gradient(315deg, #2a2a72 0%, #009ffd 74%);
}
</style>
