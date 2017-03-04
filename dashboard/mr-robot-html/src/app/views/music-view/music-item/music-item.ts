import {MusicService} from "../../service/music.service";
export class MusicItem {

  private static idCounter : number = 0;
  private id : number;
  constructor(private title:string, private artist:string, private image:string){
    this.id = MusicItem.idCounter++;
  }

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
    return MusicService.DEFAULT_PATH +
      (this.image.trim().length == 0 || this.image == null ? MusicService.DEFAULT_IMAGE : this.image);
  }

  public set setImage(value : string) {
    this.image = value;
  }

  public get getId() {
    return this.id;
  }
}
