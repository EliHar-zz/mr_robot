import { MrRobotPage } from './app.po';

describe('mr-robot App', function() {
  let page: MrRobotPage;

  beforeEach(() => {
    page = new MrRobotPage();
  });

  it('should display message saying app works', () => {
    page.navigateTo();
    expect(page.getParagraphText()).toEqual('mr works!');
  });
});
