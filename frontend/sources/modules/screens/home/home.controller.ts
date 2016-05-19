module app {

  'use strict';

  /**
   * Displays the home screen.
   */
  export class HomeController {

    isLoading: boolean = true;
    quote: string = null;

    private logger: ILogger;
    private quoteService: QuoteService;

    constructor(logger: LoggerService,
                quoteService: QuoteService) {

      this.logger = logger.getLogger('home');
      this.quoteService = quoteService;
      this.loadQuote();

      this.logger.log('init');

    }

    loadQuote() {
      this.isLoading = true;
      this.quoteService
        .getRandomJoke()
        .then((quote: string) => {
          this.quote = quote;
        })
        .finally(() => {
          this.isLoading = false;
        });
    }
  }

  angular
    .module('app')
    .controller('homeController', HomeController);

}

