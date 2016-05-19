module app {

  'use strict';

  /**
   * Quote service: allows to get quote of the day.
   */
  export class QuoteService {

    private ROUTES = {
      randomJoke: '/jokes/random?escape=javascript'
    };

    constructor(private $q: ng.IQService,
                private restService: RestService,
                private contextService: ContextService) {
    }

    /**
     * Get a random Chuck Norris joke.
     * Used context properties:
     * - category: the joke's category: 'nerdy', 'explicit'...
     * @param {!Object} context The context to use.
     * @return {Object} The promise.
     */
    getRandomJoke(): ng.IPromise<string> {
      return this.restService
        .get(this.ROUTES.randomJoke, null, false)
        .then((response: any) => {
          if (response.data && response.data.value) {
            return response.data.value.joke;
          }
          return this.$q.reject();
        })
        .catch(function() {
          return 'Error, could not load joke :-(';
        });
    }

  }

  angular
    .module('app')
    .service('quoteService', QuoteService);

}
