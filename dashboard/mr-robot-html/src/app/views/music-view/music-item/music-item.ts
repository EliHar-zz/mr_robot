import {MusicService} from "../../service/music.service";
export class MusicItem {

  constructor(private title:string, private artist:string, private image:string){}

  public get getTitle(): string {
    return this.title;
  }

  public set setTitle(value: string) {
    this.title = value;
  }

  public get getArtist(): string {
    return this.artist;
  }

  public set setArtist(value: string) {
    this.artist = value;
  }

  public get getImage() {
    return this.image.length == 0 || this.image == null ? MusicService.DEFAULT_IMAGE : this.image;
  }

  public set setImage(value : string) {
    this.image = value;
  }
}
